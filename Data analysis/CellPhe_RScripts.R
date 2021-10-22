## segmentation error removal ##

## import files containing ground truth examples of correct segmentation and segmentation errors
Segerrortraining <- read.delim("~/Desktop/DataForCellPhe/SegmentationErrors_training.txt") 
Correctsegtraining <- read.delim("~/Desktop/DataForCellPhe/CorrectSegmentation_training.txt")

## collate correct segmentation and segmentation error data into one data frame
data = rbind(Segerrortraining, Correctsegtraining) 
segdata<-Segerrortraining[,-c(1,2,3)] 
nonsegdata<-Correctsegtraining[,-c(1,2,3)]
alldata = rbind(segdata, nonsegdata)

## first column lists ground truth data labels
class1 = rep("segerror", dim(segdata)[1])
class2 = rep("correct", dim(nonsegdata)[1]) 
class = c(class1, class2)
all = data.frame(class, alldata) 

## install smotefamily package
install.packages('smotefamily') 
library(smotefamily)

## SMOTE() to over-sample segmentation errors 
smoteseg<-SMOTE(all[,-1], all$class, K = 3, dup_size = 1) 

## add smote segmentation errors to the training sets and update segmentation error data table
smotesegsyn_data<-smoteseg$syn_data[,c(703, 1:702)] 
all<-rbind(all, smoteseg$syn_data) 
segdata<-rbind(segdata, smotesegsyn_data[,-1]) 
newclass<-c(class, smotesegsyn_data[,1]) 
class1 = rep("segerror", dim(segdata)[1])

## install tree package
install.packages('tree') 
library(tree)

## define multitree function
multitree = function (smalldata, bigdata, smallclass, bigclass, num) 
{ 
  n1 = length(smallclass)
  n2 = length(bigclass)
  treelist = list()
  for (i in 1:num)
  { 
    inds = sample.int(n2, n1)
    data = rbind(bigdata[inds,],smalldata)
    class = c(bigclass[1:n1], smallclass)
    data = data.frame(class, data)
    mytree = tree(as.factor(class)~., data=data)
    treelist[[i]] <- mytree 
  } 
  return(treelist) 
}

## define multipred function
multipred = function (data, trees, num) 
{ 
  pred = matrix(" ", nrow = num, ncol = nrow(data)) 
  for (i in 1:num)
  { 
    x = predict(trees[[i]], data, type = "class")
    pred[i,] <- x 
  } 
  return(pred) 
}

## train 50 decision trees using training set
trees = multitree(segdata, nonsegdata, class1, class2, 50)

## import test set and classify
Segmentationtestset <- read.delim("~/Desktop/DataForCellPhe/Segmentation_testset.txt") 
alltest<-Segmentationtestset
predtest = multipred(alltest[,-c(1,2,3)], trees, 50)

## obtain list of cell names for cells that were predicted as segmentation error
testnumseg = vector(mode = "integer", length = ncol(predtest)) 
for (i in 1:ncol(predtest))
{ 
  testnumseg[i] = length(which(predtest[,i] == 2)) 
} 
testvote = rep("nonseg", length = ncol(predtest))

## decision tree votes combined to produce final predicted label for each cell 
## (>= 35/50 votes for segmentation error)

for (i in 1:ncol(predtest))
{ 
  if (testnumseg[i] > 3.5 * nrow(predtest)/5) testvote[i] = "seg" 
} 
ind = which(testvote == "seg")
segtest = Segmentationtestset$cellname[ind]

## save predicted segmentation error cell names as a .txt file 
write.table(segtest, "Predictedsegerrors.txt", row.names = F)

## create empty list to store decision tree votes for each cell
votes<-list()

## train ten sets of 50 decision trees
## votes[[i]] contains predictions from the ith set of decision trees 
for(i in c(1:10))
{ 
  trees = multitree(segdata, nonsegdata, class1, class2, 50)
  predtest = multipred(alltest[,-c(1,2,3)], trees, 50)
  testnumseg = vector(mode = "integer", length = ncol(predtest)) 
  for (j in 1:ncol(predtest)) 
  { 
    testnumseg[j] = length(which(predtest[,j] == 2)) 
    
  } 
  testvote = rep("nonseg", length = ncol(predtest)) 
  for (j in 1:ncol(predtest))
  { 
    if (testnumseg[j] > 3.5* nrow(predtest)/5) testvote[j] = "seg" 
  } 
  
  ind = which(testvote == "seg")
  segtest = Segmentationtestset$cellname[ind] 
  print(i)
  votes[[i]]<-segtest 
} 

## identifies cells that receive >= 5 votes for segmentation error ## and stores these in segtest
list<-NULL
for(i in c(1:10)) 
{ 
  list<-c(list, as.character(votes[[i]])) 
} 

segtest<-vector()
j=1 
freqdata<-as.data.frame(table(list)) 
for(i in c(1:dim(freqdata)[1]))
{ 
  if(freqdata$Freq[i] >= 5) 
  { 
    segtest[j] = as.character(freqdata$list[i]) 
    j=j+1 
  }
}

## save predicted segmentation error cell names as a .txt file 
write.table(segtest, "Predictedsegerrors.txt", row.names = F)

## remove predicted segmentation errors from test set ahead of treatment classification
FullTestSet<-subset(Segmentationtestset, Segmentationtestset$cellname %in% segtest == FALSE)


## ensemble classification ##

## import training set (identified segmentation errors removed)
FullTrainingSet<-read.delim("~/Desktop/DataForCellPhe/TreatmentClassification_training.txt")

dataforscaling<-rbind(FullTrainingSet[,-c(1,2,3)], FullTestSet[,-c(1,2,3)])
dataforscaling<-scale(dataforscaling)
FullTrainingSet[,-c(1,2,3)]<-dataforscaling[c(1:nrow(FullTrainingSet)),]
FullTestSet[,-c(1,2,3)]<-dataforscaling[-c(1:nrow(FullTrainingSet)),]

## import file of separation scores
separationscores <- read.table("~/Desktop/DataForCellPhe/separationscores.txt", quote="\"", comment.char="")

## subset separation scores file to only include variables with separation >= 0.2
chosenvars0.2<-subset(separationscores, separationscores$V3 >= 0.2)

## subset training and test sets to only include variables with separation >= 0.2
SelectedTrainingSet<-FullTrainingSet[,c((chosenvars0.2$V1)+4)] 
SelectedTestSet<-FullTestSet[,c((chosenvars0.2$V1)+4)]

## install packages used for classification
install.packages("MASS") 
library(MASS)
install.packages("randomForest") 
library(randomForest)
install.packages("e1071") 
library(e1071)

## classifier training
ldamodel<-lda(SelectedTrainingSet, FullTrainingSet$Group)
rfmodel <- randomForest(FullTrainingSet$Group~., data = SelectedTrainingSet, ntree=200, mtry=5, importance=TRUE, norm.votes = TRUE)
svmmodel<-svm(SelectedTrainingSet, FullTrainingSet$Group, kernel = 'radial', probability = TRUE)
## classifier testing
ldapred = predict(ldamodel, SelectedTestSet)
rfpred = predict(rfmodel, SelectedTestSet)
svmpred = predict(svmmodel, SelectedTestSet)

## ensemble classification, final predicted label based on majority vote
classificationvotes<-cbind(as.character(ldapred$class), as.character(rfpred), as.character(svmpred))

classificationvotes<-as.data.frame(classificationvotes)

for(i in c(1:dim(FullTestSet)[1])) 
{
  if((classificationvotes[i,1]=="A" && classificationvotes[i,2]=="A") ||(classificationvotes[i,1]=="A" && classificationvotes[i,3]=="A") 
     ||(classificationvotes[i,2]=="A" && classificationvotes[i,3]=="A")) 
    {
      classificationvotes[i,4] = "A" 
    }
  else
  {
    classificationvotes[i,4] = "B" 
  }
}

colnames(classificationvotes)<-c("LDA", "RF", "SVM", "Ensemble")

## confusion matrix of ensemble classification results
table(true = FullTestSet$Group, predicted = classificationvotes$Ensemble)

## clustering ##

## install factoextra for hierarchical clustering and plotting dendogram
install.packages("factoextra") 
library(factoextra)

## subset of just training set treated cells
TreatedCells<-subset(FullTrainingSet, FullTrainingSet$Group == "B")

## distance matrix and hierarchical clustering
d<-dist(scale(TreatedCells[,-c(1,2,3)]), method = "euclidean")
hierclust<-hcut(d, hc_func = "agnes", hc_method = "ward.D", hc_metric = "euclidean", k = 6)
fviz_dend(hierclust, show_labels = FALSE)+theme_classic(base_size = 20)


